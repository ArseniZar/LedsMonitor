<template>
    <div className="flex justify-center items-center mt-10 w-full">
        <div ref="pickerRef" class="w-full"></div>
    </div>
    <ColorMainBox v-bind:colorBox="device.boxColorMain" v-on:onSelect="onSelect" />

    <div class="flex  w-full flex-row justify-between gap-3 mt-5 mx-auto mb-5">
        <ColorBox v-for="box in device.boxColorsList" v-bind:colorBox="box" v-on:onSelect="onSelect" />
    </div>

    <div class="w-full flex items-center mt-5 mx-auto mb-5">
        <DeleteBox v-bind:value="0" v-on:onSelect="onSelect"/>
    </div>
</template>

<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import iro from '@jaames/iro';
import type { Device } from '../../interface';
import ColorMainBox from './panelElements/ColorMainBox.vue';
import ColorBox from './panelElements/ColorBox.vue';
import offBox from './panelElements/offBox.vue';
export default defineComponent({
    name: 'ColorPanel',
    components: {
        ColorMainBox,
        ColorBox,
        offBox,
    },
    emits: ['colorValue', 'device', 'status'],
    props: {
        device: {
            type: Object as PropType<Device>,
            required: true
        },
    },
    data(): {
        status: boolean
        colorValue: string;
        currentNumberBox: number;
        _resizeObserver: ResizeObserver | null;
    } {
        return {
            status: false,
            colorValue: '#e5e5e5',
            currentNumberBox: -1,
            _resizeObserver: null
        }
    },

    watch: {
        colorValue() {
            this.updateSelectedBoxColor();
            this.$emit("colorValue", this.colorValue);
        },
        status() {
            this.$emit("status", this.status);
        }
    },

    mounted() {
        this.initColorPicker();
        this.status = false;
        this.currentNumberBox = 0;
        window.addEventListener('beforeunload', this.handleBeforeUnload);

    },

    methods: {
        onSelect(numberBox: number) {
            this.currentNumberBox = numberBox;
            this.status = Number(numberBox) !== 0;
        },

        updateSelectedBoxColor() {
            if (!this.currentNumberBox) return;
            let box = this.device.boxColorsList.find(b => Number(b.numberBox) === Number(this.currentNumberBox)) ||
                (Number(this.device.boxColorMain.numberBox) === Number(this.currentNumberBox) ? this.device.boxColorMain : null);

            if (box && box.color !== this.colorValue) {
                box.color = this.colorValue;
            }

        },



        handleBeforeUnload() {
            this.$emit("device", this.device);
        },

        initColorPicker() {
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
        },
    },

    beforeUnmount() {
        if (this._resizeObserver) {
            this._resizeObserver.disconnect();
        }
        window.removeEventListener('beforeunload', this.handleBeforeUnload);
    },


});
</script>
