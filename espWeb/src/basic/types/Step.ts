// prettier-ignore
export type StepWifiSetupAuto =
  | "start"
  | "searchNetwork"
  | "password"
  | "check"
  | "end";
export const StepWifiSetupAuto = {
  Start: "start" as StepWifiSetupAuto,
  SearchNetwork: "searchNetwork" as StepWifiSetupAuto,
  Password: "password" as StepWifiSetupAuto,
  Check: "check" as StepWifiSetupAuto,
  End: "end" as StepWifiSetupAuto,
};

// prettier-ignore
export type StepWifiSetupManual = 
  | "start"
  | "wifiAndPassword" 
  | "check" 
  | "end";
export const StepWifiSetupManual = {
  Start: "start" as StepWifiSetupManual,
  WifiAndPassword: "wifiAndPassword" as StepWifiSetupManual,
  Check: "check" as StepWifiSetupManual,
  End: "end" as StepWifiSetupManual,
};

// prettier-ignore
export type StepWifiSetupConnect = 
  | "connect"
  | "end";
export const StepWifiSetupConnect = {
  Connect: "connect" as StepWifiSetupConnect,
  End: "end" as StepWifiSetupConnect,
};

// prettier-ignore
export type StepWifiSetup = 
  | "start" 
  | "connect" 
  | "end";
export const StepWifiSetup = {
  Start: "start" as StepWifiSetup,
  Connect: "connect" as StepWifiSetup,
  End: "end" as StepWifiSetup,
};

// prettier-ignore
export type SelectedOption = 
  | "option1" 
  | "option2"
export const SelectedOption = {
  Option1: "option1" as SelectedOption,
  Option2: "option2" as SelectedOption,
};

// prettier-ignore
export type Status = 
  | "loading" 
  | "ok" 
  | "error";
export const Status = {
  Loading: "loading" as Status,
  Ok: "ok" as Status,
  Error: "error" as Status,
}; 

