
SECRET_KEY = "d9c9b2f098519361"
DB_NAME = "JBOURDE2_ciroh"
DB_USER = "jbourde2_writer"
DB_PASSWORD = "6oCPLh7zQNIe"
DB_HOST = "webdb5.uvm.edu"
SSL_ARGS = {"ssl": {"ca": "webdb-cacert.pem"}}
DB_CONFIG = "mysql+pymysql://" + DB_USER \
    + ":" + DB_PASSWORD \
    + "@" + DB_HOST \
    + "/" + DB_NAME \
    + "?charset=utf8mb4" \
    + "&ssl_ca=" + SSL_ARGS["ssl"]["ca"]
