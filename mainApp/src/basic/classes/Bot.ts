export class Bot {
  token: string;
  name: string;
  botId: string;

  // Перегрузка конструктора: если передан объект Bot, создаём копию
  constructor(token: string, name: string, botId: string);
  constructor(bot: Bot);
  constructor(tokenOrBot: string | Bot, name?: string, botId?: string) {
    if (typeof tokenOrBot === 'string') {
      // Обычный конструктор с параметрами
      this.token = tokenOrBot;
      this.name = name || '';
      this.botId = botId || '';
    } else {
      // Конструктор копирования
      this.token = tokenOrBot.token;
      this.name = tokenOrBot.name;
      this.botId = tokenOrBot.botId;
    }
  }

  getToken(): string {
    return this.token;
  }

  getName(): string {
    return this.name;
  }

  getBotId(): string {
    return this.botId;
  }

  hasAll(): boolean {
    return Boolean(this.token !== '' && this.name !== '' && this.botId !== '');
  }

  clone(): Bot {
    return new Bot(this);
  }
}
