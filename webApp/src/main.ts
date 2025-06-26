import { createApp } from 'vue'
import './style.css'
import App from './App.vue'
import { init } from '@telegram-apps/sdk-vue';

const app = createApp(App);

try {
  init(); // Вызываем init синхронно
  app.mount('#app');
} catch (error) {
  console.error('Ошибка инициализации Telegram SDK:', error);
  app.mount('#app'); // Монтируем приложение даже при ошибке
}
