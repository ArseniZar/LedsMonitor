import { Device } from "../basic/classes/Device";

export const TELEGRAM_COMMANDS = {
  scan: "scan",
  update: "update",
} as const;

export function formatDeviceInfo(device: Device): string {
  const box = device.updateNumberBox();
  if(box){
    console.log(box?.color);
  }
  return `/command=${TELEGRAM_COMMANDS.update};id=${device.id};color=${box?.color};status=${device.status ? "on" : "off"}/`;
}

export function formatScan(): string {
  return `/command=${TELEGRAM_COMMANDS.scan};id=FF:FF:FF:FF:FF:FF/`;
}
