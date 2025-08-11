import time
from flask import Flask, Response, jsonify, request
from flask_cors import CORS

app = Flask(__name__)
CORS(app)



@app.route("/network", methods=["GET", "POST", "OPTIONS"])
def network():
    if request.method == "POST":
        data = request.get_json()
        print(data)
        time.sleep(4)
        sample_networks = {
            "ok": True,
            "result": {
                "ssid": "HomeWiFi",
                "password": "",
                "rssi": -40,
                "encryptionType": 2,
                "channel": 6,
                "bssid": "00:11:22:33:44:55",
                "hidden": False,
            } 
        }
        return jsonify(sample_networks)
    
    return jsonify({"message": "Используй POST, чтобы отправить данные."})


@app.route("/scan" ,methods=["GET"])
def networks():
    time.sleep(0)
    sample_networks = {
        "ok": True,
        "result": [
            {
                "ssid": "HomeWiFi",
                "password": "",
                "rssi": -40,
                "encryptionType": 2,
                "channel": 6,
                "bssid": "00:11:22:33:44:55",
                "hidden": False,
            },
            {
                "ssid": "CafeNet",
                "password": "",
                "rssi": -70,
                "encryptionType": 2,
                "channel": 1,
                "bssid": "AA:BB:CC:DD:EE:FF",
                "hidden": False,
            },
            {
                "ssid": "CafeNrr'",
                "password": "",
                "rssi": -80,
                "encryptionType": 2,
                "channel": 1,
                "bssid": "AA:BB:CC:DD:EE:FF",
                "hidden": False,
            },
            {
                "ssid": "CafeNrr'",
                "password": "",
                "rssi": -80,
                "encryptionType":2,
                "channel": 1,
                "bssid": "AA:BB:CC:DD:EE:FF",
                "hidden": False,
            },
            {
                "ssid": "CafeNrr'",
                "password": "",
                "rssi": -80,
                "encryptionType": 2,
                "channel": 1,
                "bssid": "AA:BB:CC:DD:EE:FF",
                "hidden": False,
            },
            {
                "ssid": "CafeNrr'",
                "password": "",
                "rssi": -80,
                "encryptionType": 2,
                "channel": 1,
                "bssid": "AA:BB:CC:DD:EE:FF",
                "hidden": False,
            },
        ],
    }

    return jsonify(sample_networks)

@app.route("/end" ,methods=["GET"])
def end():
    return Response(status=204)
    
    
if __name__ == "__main__":
    app.run(debug=True, host="localhost", port=8080)
