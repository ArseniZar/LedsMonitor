import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import tailwindcss from '@tailwindcss/vite'


export default defineConfig({
  base :'/Lumvia/',
  plugins: [vue(),tailwindcss(),],
  server: {
    host: true,
    port: 5173,
    allowedHosts: ['b9b5f1acf1b2.ngrok-free.app']
  }
})
