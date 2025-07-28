// src/utils/imagePreloader.ts

type OnAllLoadedCallback = () => void;

const preloadedImages: Record<string, HTMLImageElement> = {};
let imagesToLoad = 0;
let imagesLoaded = 0;
let onAllLoadedCallback: OnAllLoadedCallback | null = null;

export function preloadImages(urls: string[], onAllLoaded: OnAllLoadedCallback): void {
    imagesToLoad = urls.length;
    imagesLoaded = 0;
    onAllLoadedCallback = onAllLoaded;

    urls.forEach(url => {
        const img = new Image();
        img.src = url;
        preloadedImages[url] = img;

        img.onload = () => {
            imagesLoaded++;
            checkAllLoaded();
        };

        img.onerror = () => {
            imagesLoaded++;
            checkAllLoaded();
        };
    });
}

function checkAllLoaded(): void {
    if (imagesLoaded >= imagesToLoad && onAllLoadedCallback) {
        onAllLoadedCallback();
    }
}

export function getPreloadedImage(url: string): HTMLImageElement | undefined {
    return preloadedImages[url];
}
