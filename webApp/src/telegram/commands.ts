import type { Device } from "../interface";

export const TELEGRAM_COMMANDS = {
  scan: 'scan',
  update: 'update'
} as const;



export function formatDeviceInfo(device: Device): string {
  const { currentNumberBox, boxColorMain, boxColorOff, boxColorsList } = device;

  const priorityBoxes = [boxColorMain, boxColorOff];

  const color =
    priorityBoxes.find(box => box.numberBox === currentNumberBox)?.color ||
    boxColorsList.find(
      (bc) => bc.numberBox === currentNumberBox && bc.numberBox !== boxColorOff.numberBox
    )?.color || "";

  return `/command:${TELEGRAM_COMMANDS.update} mac:${device.id} chat:${device.chat.id} color:${color} status:${device.status ? "on" : "off"}`;
}


export function formatScan(): string {
  return `/command:${TELEGRAM_COMMANDS.scan}`
}
