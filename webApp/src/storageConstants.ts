export const STORAGE_KEYS = {
  currentDevice: null
};

export function saveStorageKeys(data: typeof STORAGE_KEYS): void {
  localStorage.setItem('APP_STORAGE_KEYS', JSON.stringify(data));
}

export function loadStorageKeys(): typeof STORAGE_KEYS | null {
  const json = localStorage.getItem('APP_STORAGE_KEYS');
  if (!json) return null;
  return JSON.parse(json);
}