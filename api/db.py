import sqlite3
from sqlite3.dbapi2 import Cursor

DATABASE_NAME = "airquality.sqlite"

def get_db():
    conn = sqlite3.connect(DATABASE_NAME)
    return conn

def create_tables():
    sql_query = """ CREATE TABLE IF NOT EXISTS airquality(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            location TEXT NOT NULL,
            reading REAL NOT NULL,
            timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    )
    """
    db = get_db()
    cursor = db.cursor()
    cursor.execute(sql_query)

def insert_device(location, reading):
    db = get_db()
    cursor = db.cursor()
    statement = "INSERT INTO airquality(location, reading) VALUES (?, ?)"
    cursor.execute(statement, [location, reading])
    db.commit()
    return True

def get_devices():
    db = get_db()
    cursor = db.cursor()
    statement = "SELECT * FROM airquality"
    cursor.execute(statement)
    return cursor.fetchall()