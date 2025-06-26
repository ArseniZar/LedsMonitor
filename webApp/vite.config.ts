import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import tailwindcss from '@tailwindcss/vite'
// import  from '@types/telegram-web-app'
// https://vite.dev/config/
export default defineConfig({
  base :'/LedsMonitor/',
  plugins: [vue(),tailwindcss(),],
})
