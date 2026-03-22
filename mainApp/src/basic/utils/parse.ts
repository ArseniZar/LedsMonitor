class ParseError extends Error {
  command: string;
  constructor(message: string, command: string) {
    super(message);
    this.command = command;
    this.name = 'ParseError';
  }
}


/** Парсит MAC-адрес из строки в формате "/command=scan;id=XX:XX:XX:XX:XX:XX;name=SomeName/" */
export function parseMac(text: string): string {
  const regex = /id=([0-9A-Fa-f]{2}(:[0-9A-Fa-f]{2}){5})/i;
  const match = text.match(regex);
  if (!match) {
    throw new ParseError('Failed to parse MAC address from text', 'mac');
  }
  return match[1].toUpperCase();
}

/** Парсит имя из строки в формате "/command=scan;id=XX:XX:XX:XX:XX:XX;name=SomeName/" */
export function parseName(text: string): string {
  const regex = /name=([^;\/]+)/i;
  const match = text.match(regex);
  if (!match) {
    throw new ParseError('Failed to parse NAME from text', 'name');
  }
  return match[1].trim();
}