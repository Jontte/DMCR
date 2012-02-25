# Smart Pointers: miksi ja miten

DMCR-backend käyttää uuden C++11-standardin mukaisesti perinteisten "alastomien" pointtereiden
("naked pointer") sijasta "smart pointereita" `std::shared_ptr` ja `std::unique_ptr`. Näiden
avulla on helpompi välttää pointterien ympäriinsä heittelemisestä seuraavia virheitä ja ne
kuvastavat pointerien sisältämän datan omistajuutta.

Nyrkkisääntö on, että olisi hyvästä jos koodissa ei ole lainkaan `new`:iä tai `delete`:ä.

Smart pointerit löytyvät headerista `<memory>` lukuunottamatta `dmcr::make_unique`:ta
joka ei ole vielä päässyt standardiin. (mutta on pääsevä.) Se löytyy projektin sisältämästä
headerista `"unique_ptr"`.

## std::shared_ptr

`std::shared_ptr` on "reference-counted" pointteri. Sitä voi kopioida vapaasti ympäri ohjelmaa
ja objekti, johon pointteri osoittaa vapautetaan vasta kun kaikki viittaukset ovat kadonneet.
Yleisesti tämä tarkoittaa sitä, että `std::shared_ptr`:n sisältämällä datalla ei ole varsinaista
omistajaa, vaan dataa voivat käyttää eri paikat ohjelmassa tasa-arvoisesti.

Backendissä on `typedef`attu eri luokat tyyliin `std::shared_ptr<A>` on
`APtr`.

### Milloin

Kun pointterin osoittamalla objektilla ei ole selkeää omistajaa jonka elinaika on todistettavasti
pidempi kuin objektia käyttävien koodinosien elinaika. DMCR:ässä tällaisia luokkia ovat
ainakin Scene ja RenderResult joita voivat käyttää monet koodinosat täysin erillisesti.

### Miten

Luo objekti funktiolla: `auto a = std::make_shared<A>(x,y,z);` jossa `A` on luokan nimi ja
`x`, `y` ja `z` luokan konstruktorille annettavat parametrit.

### Tärkeitä huomioita

Pointeria ei tarvitse siis vapauttaa manuaalisesti `delete`:llä, vaan kun koodi poistuu lohkosta
jossa pointeria käytetään, se vapautuu automaattisesti. Jos pointer on luokan instanssimuuttuja,
se vapautetaan kun luokka tuhoutuu.

*HUOM!* Jos tarvitset objektia vain väliaikasesti metodissa, on kannattavaa ottaa se referenssillä
`auto& a = ...` tai `const auto& a = ...`. Jos kuitenkin haluat tallentaa pointerin instanssimuuttujaan,
ota siitä kopio `auto a = ...`. Referenssillä otettaessa pointerin reference count ei nouse eikä
referenssiä siis tulkita objektin käytöksi jolloin jos pointer elää funktiokutsun ulkopuolelle,
voi syntyä hauskoja segfaultteja y.m.

## std::unique_ptr

`std::unique_ptr` on pointteri jolla on selkeästi määritelty omistaja. Kaikkien koodinosien,
jotka käyttävät pointteria, elinaika täytyy olla omistajan elinajan subset. Yksikäsitteisen
omistajan varmistamiseksi `std::unique_ptr`:ää ei voi kopioida normaalisti, vaan kopiointi
tyyliin `std::unique_ptr<int> a = b;` jossa b on tyyppiä `std::unique_ptr<int>` aiheuttaa
käännösaikaisen virheen. Tämä tarkoittaa sitä, että kyseisen pointerin omistaa koodinosa, jossa
`b` on määritelty, siis joku toinen luokka tai metodi. `std::unique_ptr`:n osoittama muisti
vapautetaan, kun _sen omistaja tuhoutuu_.

Omistajaa voi vaihtaa funktiolla `std::move(...)`. Otetaan esimerkki:

    class A {
    public:
      std::unique_ptr<int> my_ptr;

      A() { my_ptr = make_unique<int>(42); }
    }

Nyt `my_ptr`:n osoittama muisti vapautuu kun luokan A instanssi tuhoutuu. Lisätään funktio:

    int main() {
      A a;
      std::unique_ptr<int> ptr = a.my_ptr;
    }

Tämä ei mene kääntäjästä läpi, sillä `my_ptr`:n omistaja on A:n instanssi ja `main`
yrittää kopioida pointerin noin vain. Kun kuitenkin muutetaan koodia seuraavasti

    int main() {
      A a;
      std::unique_ptr<int> ptr = std::move(a.my_ptr);
    }

se kääntyy ja toimii. Alkuperäisessä koodissa `my_ptr`:n osoittama muisti tuhoutui
kun instanssi `a` tuhoutui. Nyt kuitenkin pointterin omistus siirtyi `a`:lta
`main`-funktiolle joten muisti vapautuu kun `main` palaa.

*HUOM!* `a`:n instanssimuuttuja `my_ptr` on nyt NULL-pointteri! Pointterin
entinen omistaja ei voi enää käyttää pointteria jonka omistuksen se on menettänyt.
(ellei se hanki sitä uudelleen "naked pointer"ina)

Jossain vaiheessa kuitenkin epäilemättä tulee tarve päästä käsiksi unique_ptr:ien
sisältämään dataan muista luokista jotka eivät omista kyseistä dataa. Tällöin
voidaan kutsua `std::unique_ptr`:n metodia `get()` joka palauttaa "naked pointer"in.
Esimerkiksi:

    class A {
    public:
      A* ptr() { return my_ptr.get(); }
    }

    int main() {
      A a;
      int* i = a.ptr();
    }

Tällöin int-objektin omistus ei siirry pois `a`:lta mutta `main` pystyy kuitenkin
käyttämään sitä. Täytyy luonnollisesti muistaa että näitä "naked pointer"eita
ei saa vapauttaa manuaalisesti. Omistus säilyy alkuperäisellä objektilla
joten muisti vapautuu aikanaan.

### Milloin

Kun objektilla on selkeä omistaja jonka tehtävä on kontrolloida kyseistä dataa.
DMCR:ssä näitä on ainakin SceneObject (Scene-luokat omistavat kaikki omat SceneObjectinsa,
ei ole mitään järkeä olla olemassa SceneObjecteja ilman relevanttia Scene-luokkaa)
ja Task jonka omistaa TaskManager.

### Miten

Luo objekti funktiolla: `auto a = dmcr::make_unique<A>(x,y,z)` vastaavasti kuin
`std::shared_ptr`:kin. Muuta objektin omistajaa laittamalla sen ympärille `std::move()`
paikassa, johon omistus siirtyy. Ota muualle leviteltäväksi "naked pointer"eita
metodilla `get()`.
