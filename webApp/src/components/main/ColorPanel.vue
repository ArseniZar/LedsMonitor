<template>
    <div className="flex justify-center items-center mt-10 w-full">
        <div ref="pickerRef" class="w-full"></div>
    </div>
    <ColorMainBox v-bind:colorBox="localDevice.boxColorMain" v-bind:modelValue="localDevice.currentNumberBox.valueOf()"
        v-on:onSelect="onSelect" />

    <div class="flex  w-full flex-row justify-between gap-3 mt-5 mx-auto mb-5">
        <ColorBox v-for="box in localDevice.boxColorsList" v-bind:colorBox="box"
            v-bind:modelValue="localDevice.currentNumberBox.valueOf()" v-on:onSelect="onSelect" />
    </div>

    <div class="w-full flex items-center mt-5 mx-auto mb-5">
        <OffBox v-bind:value="0" v-bind:modelValue="localDevice.currentNumberBox.valueOf()" v-on:onSelect="onSelect" />
    </div>
</template>

<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import iro from '@jaames/iro';
import type { Device } from '../../interface';
import ColorMainBox from './panelElements/ColorMainBox.vue';
import ColorBox from './panelElements/ColorBox.vue';
import OffBox from './panelElements/OffBox.vue';
export default defineComponent({
    name: 'ColorPanel',
    components: {
        ColorMainBox,
        ColorBox,
        OffBox,
    },
    emits: ['updateDevice'],
    props: {
        device: {
            type: Object as PropType<Device>,
            required: true
        },
    },
    data(): {
        localDevice: Device,
        colorValue: string;
        _resizeObserver: ResizeObserver | null;
    } {
        return {
            localDevice: {} as Device,
            colorValue: '#e5e5e5',
            _resizeObserver: null
        }
    },

    watch: {
        device: {
            immediate: true,
            handler(newDevice) {
                this.localDevice = JSON.parse(JSON.stringify(newDevice));
                console.log(typeof newDevice.currentNumberBox);
                console.log(this.localDevice)
            }
        },

        // localDevice(){
        //     this.updateDevice();
        // }

        colorValue() {
            this.updateSelectedBoxColor();
        },
    },

    mounted() {
        this.initColorPicker();
        this.localDevice.currentNumberBox = Number(0);
        this.updateDevice();
    },

    methods: {
        onSelect(numberBox: number) {
            this.localDevice.currentNumberBox = Number(numberBox);
            this.localDevice.status = numberBox !== 0;
            this.updateDevice();
        },

        updateSelectedBoxColor() {
            if (!this.localDevice.currentNumberBox) return;
            let box = this.localDevice.boxColorsList.find(b => Number(b.numberBox) === Number(this.localDevice.currentNumberBox)) ||
                (Number(this.localDevice.boxColorMain.numberBox) === Number(this.localDevice.currentNumberBox) ? this.localDevice.boxColorMain : null);

            if (box && box.color !== this.colorValue) {
                box.color = this.colorValue;
                this.updateDevice();
            }

        },



        updateDevice() {
            this.$emit("updateDevice", this.localDevice);
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
    },


});
</script>
