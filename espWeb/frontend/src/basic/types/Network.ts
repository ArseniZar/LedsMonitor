export type EncryptionType = 0 | 1 | 2 | 3 | 4;

export const EncryptionType = {
  NONE: 0 as EncryptionType,
  WEP: 1 as EncryptionType,
  WPA: 2 as EncryptionType,
  WPA2: 3 as EncryptionType,
  WPA3: 4 as EncryptionType,
} as const;

export type SignalLevel = 1 | 2 | 3 | 4 | 5;

export const SignalLevel = {
  VeryWeak: 1 as SignalLevel,
  Weak: 2 as SignalLevel,
  Medium: 3 as SignalLevel,
  Strong: 4 as SignalLevel,
  VeryStrong: 5 as SignalLevel,
} as const;
