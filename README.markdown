
Distributed Monte-Carlo Raytracer
=================================

Authenticate with Github
--------------------
1. Create RSA-key if you don't have one already
	- ssh-keygen -t rsa -C "email@email.com"

2. Add public key to Github profile
	- copy everything from id_rsa.pub (or where ever you stored the key) to Github-> 
	your profile -> SSH Public Keys

3. Configure your local git:
	- git config --global user.name "Your name"
	- git config --global user.email "your@email.com"
	


Working with Aptana Studio 3
--------------------
1. Install Pydev (for frontend, Python coding) and CDT (for backend, C++ coding)
2. File -> Import -> Git
	Address is git@github:Jontte/DMCR.git

3. You should now have one new project:
	- DMCR
	The DMCR-project includes the other two, but to make use of Eclipse, C++ and Python
	must be own projects also.
	So we must import them as projects.
	
4. File -> Import -> General - Import existing project into workspace
	Choose DMCR/frontend or backend
	Don't allow Eclipse to copy project contents
	
	NOTE: As you import backend, Eclipse builds it automatically. (At least if you have 
	the dependencies.)
	
5.	Repeat for the other one
	Now you should have three projects:
	- backend
	- DMCR
	- frontend
	
6.	If you have problem with files and folders showing twice in Project Explorer:
	- Click on the drop-down arrow in Project Explorer
	- Select "Customize View"
	- Switch to "Content"
	- Disable "Local Filesystem"
	That should work.
	

Building the backend
--------------------

1. Make sure you have the dependencies
   - GCC + C++11 -compliant compiler
   - Google protobuf (libprotobuf-dev in Ubuntu)
   - JsonCpp (http://sourceforge.net/projects/jsoncpp/)

2. `$ make depend`

3. `$ make`

