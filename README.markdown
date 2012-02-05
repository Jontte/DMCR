
Distributed Monte-Carlo Raytracer
=================================

Authenticate with Github
--------------------
1. Create RSA-key if you don't have one already
	- ssh-keygen -t rsa -C "email@email.com"

2. Add public key to Github profile
	- copy everything from id_rsa.pub (or where ever you stored the key) to Github-> 
	your profile -> SSH Public Keys



Working with Aptana Studio 3
--------------------
1. Install Pydev (for frontend, Python coding) and CDT (for backend, C++ coding)
2. File -> Import -> Git
	Address is git@github:Jontte/DMCR.git

3. You should now have 3 new projects:
	- DMCR
	- frontend
	- backend
	The DMCR-project includes the other two, but to make use of Eclipse, C++ and Python
	must be own projects also.

Building the backend
--------------------

1. Make sure you have the dependencies
   - GCC + C++11 -compliant compiler
   - Google protobuf (libprotobuf-dev in Ubuntu)

2. `$ make depend`

3. `$ make`

