export type EncryptionType =
  | "NONE"
  | "WEP"
  | "WPA_PSK"
  | "WPA2_PSK"
  | "WPA3_PSK";

export type SignalLevel = 1 | 2 | 3 | 4 | 5;

export const SignalLevel = {
  VeryWeak: 1 as SignalLevel,
  Weak: 2 as SignalLevel,
  Medium: 3 as SignalLevel,
  Strong: 4 as SignalLevel,
  VeryStrong: 5 as SignalLevel,
} as const;
