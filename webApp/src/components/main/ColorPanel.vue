<template>
    <div className="flex justify-center items-center mt-10 w-full">
        <div ref="pickerRef" class="w-full"></div>
    </div>
    <ColorMainBox v-bind:color="boxColorMain.color" v-bind:uniqueId="boxColorMain.uniqueId" v-on:onSelect="onSelect" />

    <div class="flex  w-full flex-row justify-between gap-3 mt-5 mx-auto mb-10">
        <ColorBox v-for="box in boxColorList" v-bind:color="box.color" v-bind:uniqueId="box.uniqueId"
            v-on:onSelect="onSelect" />
    </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import iro from '@jaames/iro';
import ColorMainBox from './colorElements/ColorMainBox.vue';
import ColorBox from './colorElements/ColorBox.vue';
const boxColors = 'boxColors'
const boxColorsMain = 'boxColorsMain'
interface boxColor {
    uniqueId: string;
    color: string;
}

function generateId(): string {
    return 'color-' + Math.random().toString(36).substr(2, 5);
}

function functionloadColorsBoxFromLocalStorage() {
    const saved = localStorage.getItem(boxColors);
    if (saved) {
        return JSON.parse(saved);
    } else {
        return [{ uniqueId: generateId(), color: '#ffffff' },
        { uniqueId: generateId(), color: '#ffa500' },
        { uniqueId: generateId(), color: '#ff00ff' },
        { uniqueId: generateId(), color: '#008000' },
        { uniqueId: generateId(), color: '#8000ff' }];
    }
}
function functionloadColorsBoxMainFromLocalStorage() {
    const saved = localStorage.getItem('boxColorsMain');
    if (saved) {
        return JSON.parse(saved);
    } else {
        return { uniqueId: generateId(), color: '#e5e5e5' }
    }
}

export default defineComponent({
    name: 'ColorPanel',
    components: {
        ColorMainBox,
        ColorBox,
    },
    data(): {
        colorValue: string;
        boxColorList: boxColor[];
        selectUniqueId: string;
        boxColorMain: boxColor;
        _resizeObserver: ResizeObserver | null;
    } {
        return {
            colorValue: '#e5e5e5',
            boxColorList: functionloadColorsBoxFromLocalStorage(),
            boxColorMain: functionloadColorsBoxMainFromLocalStorage(),
            selectUniqueId: '',
            _resizeObserver: null
        }
    },
    methods: {
        onSelect(uniqueId: string) {
            this.selectUniqueId = uniqueId;
            console.log(uniqueId)
        },

        updateSelectedBoxColor() {
            if (!this.selectUniqueId) return;
            const box = this.boxColorList.find(b => b.uniqueId === this.selectUniqueId) || this.boxColorMain;
            if (box && box.color !== this.colorValue) {
                box.color = this.colorValue;
            }
        },

        saveColorsBoxToLocalStorage() {
            console.log(JSON.stringify(this.boxColorList))
            localStorage.setItem(boxColors, JSON.stringify(this.boxColorList));
        },

        saveColorsBoxMainToLocalStorage() {
            console.log(JSON.stringify(this.boxColorMain))
            localStorage.setItem(boxColorsMain, JSON.stringify(this.boxColorMain));
        },

    },
    watch: {
        colorValue() {
            this.updateSelectedBoxColor();
        }


    },

    mounted() {
        const pickerElement = this.$refs.pickerRef as HTMLElement;

        if (!pickerElement) return;

        const colorPickerInstance = iro.ColorPicker(pickerElement, {
            width: 100,
            color: this.colorValue,
            margin: 50,
            borderWidth: 0,
            padding: 8,
            handleRadius: 8,
            layout: [
                { component: iro.ui.Wheel },
                { component: iro.ui.Slider, options: { sliderType: 'value' } },
            ],
        });

        colorPickerInstance.on('color:change', (newColor: iro.Color) => {
            this.colorValue = newColor.hexString;
        });

        const updatePickerWidth = () => {
            const style = window.getComputedStyle(pickerElement);
            const paddingLeft = parseFloat(style.paddingLeft);
            const paddingRight = parseFloat(style.paddingRight);
            const width = parseFloat(style.width);
            const containerWidth = width - paddingLeft - paddingRight - 10;

            colorPickerInstance.resize(containerWidth);
        };

        const resizeObserver = new ResizeObserver(() => {
            updatePickerWidth();
        });

        resizeObserver.observe(pickerElement);

        updatePickerWidth();

        this._resizeObserver = resizeObserver;
        window.addEventListener('beforeunload', this.saveColorsBoxToLocalStorage);
        window.addEventListener('beforeunload', this.saveColorsBoxMainToLocalStorage);
    },

    beforeUnmount() {
        if (this._resizeObserver) {
            this._resizeObserver.disconnect();
        }
        window.addEventListener('beforeunload', this.saveColorsBoxToLocalStorage);
        window.addEventListener('beforeunload', this.saveColorsBoxMainToLocalStorage);
    },


});
</script>

<!-- <script>
import { onMounted } from 'vue';
import iro from '@jaames/iro';
onMounted(() => {
const container = document.getElementById('main');
const style = window.getComputedStyle(container);
const paddingLeft = parseFloat(style.paddingLeft);
const paddingRight = parseFloat(style.paddingRight);
const contentWidth = container.offsetWidth - paddingLeft - paddingRight;


const colorPicker = new iro.ColorPicker("#picker", {
width: contentWidth,
color: "#ffffff",
margin: 50,
borderWidth: 0,
padding: 8,
handleRadius: 8,
layout: [
{ component: iro.ui.Wheel },
{ component: iro.ui.Slider, options: { sliderType: 'value', } },
]
});
});

</script> -->