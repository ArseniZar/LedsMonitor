export interface Device {
    token: string;
    name: string;
    status: Boolean;
    currentNumberBox: Number;
    boxColorsList: BoxColor[];
    boxColorMain: BoxColor;
}

export interface BoxColor {
    numberBox: Number;
    color: string;
}

export type StepAddDevice = 'token' | 'name' | 'devices';

export const StepAddDevice = {
  Token: 'token' as StepAddDevice,
  Name: 'name' as StepAddDevice,
  Devices: 'devices' as StepAddDevice
};


export type StepMain = 'main' | 'select' | 'add';

export const StepMain = {
  Main: 'main' as StepMain,
  Select: 'select' as StepMain,
  Add: 'add' as StepMain,
  Welcome: 'welcome' as StepMain,
};


