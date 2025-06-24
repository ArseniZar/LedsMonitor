export class LocalDB<T extends { id?: number }> {
  private table: string;
  private static idCounters: Record<string, number> = {};

  constructor(tableName: string) {
    this.table = tableName;

    // Инициализация счётчика ID для таблицы из localStorage или 0
    const lastId = localStorage.getItem(`${this.table}_lastId`);
    LocalDB.idCounters[this.table] = lastId ? parseInt(lastId, 10) : 0;

    // Если данных для таблицы ещё нет — создаём пустой массив
    if (!localStorage.getItem(this.table)) {
      localStorage.setItem(this.table, JSON.stringify([]));
    }
  }

  private getNextId(): number {
    LocalDB.idCounters[this.table]++;
    localStorage.setItem(`${this.table}_lastId`, LocalDB.idCounters[this.table].toString());
    return LocalDB.idCounters[this.table];
  }

  private getData(): T[] {
    const json = localStorage.getItem(this.table);
    return json ? JSON.parse(json) : [];
  }

  private saveData(data: T[]): void {
    localStorage.setItem(this.table, JSON.stringify(data));
  }

  // Вставка новой записи — id генерируется автоматически
  insert(record: Omit<T, 'id'>): T {
    const data = this.getData();
    const newRecord = { ...record, id: this.getNextId() } as T;
    data.push(newRecord);
    this.saveData(data);
    return newRecord;
  }

  // Получить все записи
  getAll(): T[] {
    return this.getData();
  }

  // Найти запись по id
  getById(id: number): T | undefined {
    const data = this.getData();
    return data.find(item => item.id === id);
  }

  // Обновить запись по id
  update(id: number, updatedFields: Partial<Omit<T, 'id'>>): T | null {
    const data = this.getData();
    const index = data.findIndex(item => item.id === id);
    if (index === -1) return null;
    data[index] = { ...data[index], ...updatedFields };
    this.saveData(data);
    return data[index];
  }

  // Удалить запись по id
  delete(id: number): boolean {
    let data = this.getData();
    const initialLength = data.length;
    data = data.filter(item => item.id !== id);
    if (data.length === initialLength) return false; // ничего не удалено
    this.saveData(data);
    return true;
  }
}