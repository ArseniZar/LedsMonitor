export class LocalDB<T extends Record<string, any>> {
  private table: string;
  private uniqueField: keyof T;

  constructor(tableName: string, uniqueField: keyof T) {
    this.table = tableName;
    this.uniqueField = uniqueField;
    if (!localStorage.getItem(this.table)) {
      localStorage.setItem(this.table, JSON.stringify([]));
    }
  }

  private getData(): T[] {
    const json = localStorage.getItem(this.table);
    return json ? JSON.parse(json) : [];
  }

  private saveData(data: T[]): void {
    localStorage.setItem(this.table, JSON.stringify(data));
  }

  insert(record: T): T {
    const data = this.getData();
    // Проверяем, нет ли уже записи с таким уникальным полем
    const exists = data.find(item => item[this.uniqueField] === record[this.uniqueField]);
    if (exists) {
      throw new Error(`Record with ${String(this.uniqueField)}="${record[this.uniqueField]}" already exists.`);
    }
    data.push(record);
    this.saveData(data);
    return record;
  }

  upsert(record: T): T {
    const data = this.getData();
    const index = data.findIndex(item => item[this.uniqueField] === record[this.uniqueField]);
    if (index === -1) {
      data.push(record);
    } else {
      data[index] = { ...data[index], ...record };
    }
    this.saveData(data);
    return record;
  }

  getAll(): T[] {
    return this.getData();
  }

  getByUnique(value: T[keyof T]): T | undefined {
    const data = this.getData();
    return data.find(item => item[this.uniqueField] === value);
  }

  deleteByUnique(value: T[keyof T]): boolean {
    let data = this.getData();
    const initialLength = data.length;
    data = data.filter(item => item[this.uniqueField] !== value);
    if (data.length === initialLength) return false;
    this.saveData(data);
    return true;
  }

  replaceAll(newData: T[]): void {
    this.saveData(newData);
  }



}


