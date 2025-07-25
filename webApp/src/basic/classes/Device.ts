import type { ItemData } from "../types/itemData";
import { Chat } from "./Chat";


export interface BoxColor {
  numberBox: Number;
  color: string;
}

export class Device {
  id: string;
  chat: Chat;
  name: string;
  status: boolean;
  currentNumberBox: number;
  boxColorsList: BoxColor[];
  boxColorMain: BoxColor;
  boxColorOff: BoxColor;
  imgIcon?: string;

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
      this.chat = chat!;
      this.name = name!;
      this.status = status!;
      this.currentNumberBox = currentNumberBox!;
      this.boxColorsList = boxColorsList!.map(box => ({ ...box }));
      this.boxColorMain = { ...boxColorMain! };
      this.boxColorOff = { ...boxColorOff! };
    } else {
      const device = idOrDevice;
      this.id = device.id;
      this.chat = new Chat(device.chat);
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

  toItemData(defaultImgIcon?: string): ItemData {
    return {
      id: this.id,
      imgIcon: this.imgIcon !== undefined ? this.imgIcon : defaultImgIcon,
      title: this.name,
    };
  }

}