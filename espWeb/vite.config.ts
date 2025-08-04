import { defineConfig } from 'vite'
import tailwindcss from '@tailwindcss/vite'
import { viteSingleFile } from 'vite-plugin-singlefile';

export default defineConfig({
    // base: '/LedsMonitor/',
    plugins: [tailwindcss(), viteSingleFile({   removeViteModuleLoader  :  true   })],
    server: {
        host: true,
        port: 5173,
        allowedHosts: ['f28b031c85b9.ngrok-free.app']
    }
})