# Flask App

Author: Jordan Bourdeau

This directory contains a basic Flask app to be ran on the UVM silk server.
The app is responsible for catching POST requests with parameters corresponding
to sensor readings gathered in the field, and sent from the Cloudloop data
platform after being received by the satellite uplink.

## Runnning

1. Setup the environment by creating a venv and installing necessary packages.

From the `flask` subdirectory in the repository, run these commands:

```
python -m venv .env
source .env/bin/activate
pip install -r requirements.txt
```

The python environment specified in the `.silk.ini` file should now be setup.

2. Place the `.silk.ini` file into the `www-root` directory within the home
directory. This configuration file specifies the environment in which the
app executes, and documentation on it can be found [here](https://silk.uvm.edu/manual/python/).

NOTE: The file assumes the following directory structure:

```
~/
	www-root/
	    .silk.ini   
        <root>
	...
```

where `<root` refers to the root of the CIROH git repository. If the internal
structure of paths within the repo changes, this path will also need to change.

3. Once the `.silk.ini` file is in this location, the app can have its new
configuration applied and be loaded (if it is the first time running the app,
it will take some time until the account for running the app is created).

```
silk site <user>.w3.uvm.edu update
silk app <user>.w3.uvm.edu load
```

If anything goes wrong, check the error logs (lookup how to do this in the Silk
documentation, as it is slightly different between V2 and V3 silk servers).

## Files

The main files for running the web app are:

* `wsgi.py`
	- Gets run by silk to start the web app
* `app.py`
    - Initializes the app and has the URL routes.
* `model.py`
    - Flask SQLAlchemy ORM schema.
* `constants.py`
    - Constants used in the app.

`cmd.sh` contains a simple Curl POST request to test route functionality.

