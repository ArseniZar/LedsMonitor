export type StepStartupApp = 'welcome' | 'token';
export const StepStartupApp = {
  Welcome: 'welcome' as StepStartupApp,
  Token: 'token' as StepStartupApp
}


export type StepAddDevice = 'chatId' | 'searchDevices' | 'devices';
export const StepAddDevice = {
  ChatId: 'chatId' as StepAddDevice,
  SearchDevices: 'searchDevices' as StepAddDevice,
  Devices: 'devices' as StepAddDevice,
};



export type StepSettings = 'settings' | 'devices' | 'token';
export const StepSettings = {
  Settings: 'settings' as StepSettings,
  Token: 'token' as StepSettings,
  Devices: 'devices' as StepSettings,
};


export type StepMain = 'main' | 'select' | 'startup' | 'settings' | 'addDevices';
export const StepMain = {
  Main: 'main' as StepMain,
  Select: 'select' as StepMain,
  Startup: 'startup' as StepMain,
  Settings: 'settings' as StepMain,
  AddDevices: 'addDevices' as StepMain
};
