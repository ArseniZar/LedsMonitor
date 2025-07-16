import type { Chat } from './telegram/types';


export class Device {
  id: string;
  chat: Chat;
  name: string;
  status: boolean;
  currentNumberBox: number;
  boxColorsList: BoxColor[];
  boxColorMain: BoxColor;
  boxColorOff: BoxColor;

  constructor(
    id: string,
    chat: Chat,
    name: string,
    status: boolean,
    currentNumberBox: number,
    boxColorsList: BoxColor[],
    boxColorMain: BoxColor,
    boxColorOff: BoxColor,
  );
  constructor(device: Device);

  constructor(
    idOrDevice: string | Device,
    chat?: Chat,
    name?: string,
    status?: boolean,
    currentNumberBox?: number,
    boxColorsList?: BoxColor[],
    boxColorMain?: BoxColor,
    boxColorOff?: BoxColor,
  ) {
    if (typeof idOrDevice === 'string') {
      this.id = idOrDevice;
      this.chat = { ...chat! };
      this.name = name!;
      this.status = status!;
      this.currentNumberBox = currentNumberBox!;
      this.boxColorsList = boxColorsList!.map(box => ({ ...box }));
      this.boxColorMain = { ...boxColorMain! };
      this.boxColorOff = { ...boxColorOff! };
    } else {
      const device = idOrDevice;
      this.id = device.id;
      this.chat = { ...device.chat };
      this.name = device.name;
      this.status = device.status;
      this.currentNumberBox = device.currentNumberBox;
      this.boxColorsList = device.boxColorsList.map(box => ({ ...box }));
      this.boxColorMain = { ...device.boxColorMain };
      this.boxColorOff = { ...device.boxColorOff };
    }
  }

  equals(other: Device): boolean {
    return this.id === other.id && this.chat.id === other.chat.id;
  }
}


export interface BoxColor {
  numberBox: Number;
  color: string;
}

export type ItemData = {
  id: number;
  imgIcon: string;
  title: string;
}

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


export type StepMain = 'main' | 'select' | 'startup' | 'settings' | 'addDevices';
export const StepMain = {
  Main: 'main' as StepMain,
  Select: 'select' as StepMain,
  Startup: 'startup' as StepMain,
  Settings: 'settings' as StepMain,
  AddDevices: 'addDevices' as StepMain
};



