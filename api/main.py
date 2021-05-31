from flask import Flask, request, jsonify
import db
from db import create_tables

app = Flask(__name__)
app.config["DEBUG"] = True

@app.route("/", methods=["GET"])
def home():
    return "<h1>Air quality API</h1>"

@app.route("/input", methods=["POST"])
def input():
    location = request.form["location"]
    reading = request.form["reading"]
    result = db.insert_device(location, reading)
    return jsonify(result)

@app.route("/input", methods=["GET"])
def get_db():
    device = db.get_devices()
    return jsonify(device)

if __name__ == "__main__":
    db.create_tables()

    app.run(host="0.0.0.0")