import type { ItemData } from "../types/ItemData";
import { EncryptionType, SignalLevel } from "../types/Network";

export class Network {
  ssid: string;
  password: string;
  rssi: number;
  encryptionType: number;
  channel: number;
  bssid: string;
  hidden: boolean;

  constructor(params: {
    ssid: string;
    password?: string;
    rssi?: number;
    encryptionType?: number;
    channel?: number;
    bssid?: string;
    hidden?: boolean;
  });

  constructor(network: Network);

  constructor(arg: any) {
    if (arg instanceof Network) {
      this.ssid = arg.ssid;
      this.password = arg.password;
      this.rssi = arg.rssi;
      this.encryptionType = arg.encryptionType;
      this.channel = arg.channel;
      this.bssid = arg.bssid;
      this.hidden = arg.hidden;
    } else {
      this.ssid = arg.ssid;
      this.password = arg.password ?? "";
      this.rssi = arg.rssi ?? 0;
      this.encryptionType = arg.encryptionType ?? EncryptionType.NONE;
      this.channel = arg.channel ?? 0;
      this.bssid = arg.bssid ?? "";
      this.hidden = arg.hidden ?? false;
    }
  }

  toItemData(imgIcon?: string): ItemData {
    return {
      id: this.ssid + this.bssid,
      title: this.ssid,
      imgIcon: imgIcon,
    };
  }

  getId() {
    return this.ssid + this.bssid;
  }

  equals(other: Network): boolean {
    if (!other) return false;
    return this.ssid === other.ssid, this.bssid === other.bssid;
  }

  getSignalLevel(): SignalLevel {
    if (this.rssi >= -50) return SignalLevel.VeryStrong;
    if (this.rssi >= -60) return SignalLevel.Strong;
    if (this.rssi >= -70) return SignalLevel.Medium;
    if (this.rssi >= -80) return SignalLevel.Weak;
    return SignalLevel.VeryWeak;
  }

  isSecured(): boolean {
    return this.encryptionType !== EncryptionType.NONE;
  }
}
