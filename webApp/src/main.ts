import { createApp } from 'vue'
import './style.css'
import App from './App.vue'

if (import.meta.env.DEV) {
    import('eruda').then(({ default: eruda }) => {
        eruda.init()
    })
}
createApp(App).mount('#app')