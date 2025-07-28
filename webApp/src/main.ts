import { createApp } from 'vue'
import './style.css'
import App from './App.vue'
import { preloadImages } from './basic/utils/imagePreloader';

const imagesToPreload = [
    'img/icons8-add-24.png',
    'img/icons8-chat-24.png',
    'img/icons8-close-52.png',
    'img/icons8-copy-24.png',
    'img/icons8-data-64.png',
    'img/icons8-dep-50.png',
    'img/icons8-device-100.png',
    'img/icons8-device-48.png',
    'img/icons8-device-50.png',
    'img/icons8-home-48.png',
    'img/icons8-key-64.png',
    'img/icons8-ped-50.png',
    'img/icons8-restart-64.png',
    'img/icons8-settings-48.png',
    'img/icons8-settings-50.png'
];

preloadImages(imagesToPreload, () => {
    if (import.meta.env.DEV) {
        import('eruda').then(({ default: eruda }) => {
            eruda.init()
        })
    }
    createApp(App).mount('#app')
});

