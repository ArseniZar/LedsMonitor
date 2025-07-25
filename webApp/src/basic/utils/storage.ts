type Constructor<T> = new (data: any) => T;

export class LocalDB<T extends Record<string, any>> {
  private table: string;
  private uniqueFieldPaths: string[];
  private ModelClass: Constructor<T>;

  constructor(tableName: string, uniqueFieldPaths: string[], ModelClass: Constructor<T>) {
    this.table = tableName;
    this.uniqueFieldPaths = uniqueFieldPaths;
    this.ModelClass = ModelClass;

    if (!localStorage.getItem(this.table)) {
      localStorage.setItem(this.table, JSON.stringify([]));
    }
  }

  private getData(): T[] {
    const json = localStorage.getItem(this.table);
    const rawData = json ? JSON.parse(json) : [];

    return rawData
      .filter((item: any) => item && typeof item === 'object')
      .map((item: any) => new this.ModelClass(item));
  }

  private saveData(data: T[]): void {
    localStorage.setItem(this.table, JSON.stringify(data));
  }

  private getValueByPath(obj: any, path: string): any {
    return path.split('.').reduce((o, key) => (o ? o[key] : undefined), obj);
  }

  private isEqualByKey(record1: T, record2: T): boolean {
    return this.uniqueFieldPaths.every(
      path => this.getValueByPath(record1, path) === this.getValueByPath(record2, path)
    );
  }

  public insert(record: T): T {
    const data = this.getData();

    if (data.find(item => this.isEqualByKey(item, record))) {
      const keyString = this.uniqueFieldPaths
        .map(path => `${path}="${this.getValueByPath(record, path)}"`)
        .join(', ');
      throw new Error(`Record with ${keyString} already exists.`);
    }

    data.push(record);
    this.saveData(data);
    return record;
  }

  public upsert(record: T): T {
    const data = this.getData();
    const index = data.findIndex(item => this.isEqualByKey(item, record));

    if (index === -1) {
      data.push(record);
    } else {
      data[index] = { ...data[index], ...record };
    }

    this.saveData(data);
    return record;
  }

  public getAll(): T[] {
    return this.getData();
  }

  public getByUniqueKey(key: Partial<T>): T | undefined {
    const data = this.getData();
    return data.find(item =>
      this.uniqueFieldPaths.every(path => this.getValueByPath(item, path) === this.getValueByPath(key, path))
    );
  }

  public deleteByUniqueKey(key: Partial<T>): boolean {
    let data = this.getData();
    const initialLength = data.length;

    data = data.filter(
      item => !this.uniqueFieldPaths.every(path => this.getValueByPath(item, path) === this.getValueByPath(key, path))
    );

    if (data.length === initialLength) return false;

    this.saveData(data);
    return true;
  }

  public replaceAll(newData: T[]): void {
    this.saveData(newData);
  }
}




export class LocalConfig<T extends Record<string, any>> {
  private key: string;

  constructor(storageKey: string) {
    this.key = storageKey;
    if (!localStorage.getItem(this.key)) {
      this.save({} as T);
    }
  }

  /** Получить весь конфиг */
  getAll(): T {
    const json = localStorage.getItem(this.key);
    return json ? JSON.parse(json) : {} as T;
  }

  /** Полностью заменить конфиг */
  setAll(config: T): void {
    this.save(config);
  }

  /** Получить одно поле по ключу */
  get<K extends keyof T>(key: K): T[K] | undefined {
    const config = this.getAll();
    return config[key];
  }

  /** Обновить одно поле */
  set<K extends keyof T>(key: K, value: T[K]): void {
    const config = this.getAll();
    config[key] = value;
    this.save(config);
  }

  /** Очистить конфиг */
  clear(): void {
    this.save({} as T);
  }

  /** Внутренний метод для сохранения */
  private save(config: T): void {
    localStorage.setItem(this.key, JSON.stringify(config));
  }
}

