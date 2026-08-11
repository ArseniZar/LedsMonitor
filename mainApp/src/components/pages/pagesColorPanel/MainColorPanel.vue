<template>
    <SelectDeviceButton :title="localDevice!.name" @selectDevice="selectDevice" />

    <div className="flex justify-center items-center mt-5 md:mt-10 g:mt-20 w-full">
        <div ref="pickerRef" class="w-full"></div>
    </div>
    <ColorMainBox :colorBox="localDevice!.boxColorMain" :modelValue="Number(localDevice!.currentNumberBox)"
        v-on:onSelect="handleSelectBox" />

    <div class="flex  w-full flex-row justify-between gap-3 mt-5  mb-5">
        <ColorBox v-for="box in localDevice!.boxColorsList" :colorBox="box"
            :modelValue="Number(localDevice!.currentNumberBox)" v-on:onSelect="handleSelectBox" />
    </div>

    <div class="w-full flex items-center mt-5 mb-5">
        <OffBox :colorBox="localDevice!.boxColorOff" :modelValue="Number(localDevice!.currentNumberBox)"
            v-on:onSelect="handleSelectBox" />
    </div>
</template>


<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import iro from '@jaames/iro';
import { Device } from '../../../basic/classes/Device';
import ColorMainBox from './pageColorPanel/ColorMainBox.vue';
import ColorBox from './pageColorPanel/ColorBox.vue';
import OffBox from './pageColorPanel/OffBox.vue';
import SelectDeviceButton from './pageColorPanel/SelectDeviceButton.vue';
import type { IroColorPicker } from '@jaames/iro/dist/ColorPicker';
export default defineComponent({
    name: 'MainColorPanel',
    components: {
        ColorMainBox,
        ColorBox,
        OffBox,
        SelectDeviceButton,
    },

    emits: ['updateDevice', 'selectDevice'],
    props: {
        device: {
            type: Object as PropType<Device>,
            required: true
        },
    },

    data(): {
        localDevice: Device | null,
        colorPicker: IroColorPicker | null,
        resizeObserver: ResizeObserver | null,
        colorValue: string,
    } {
        return {
            localDevice: null,
            colorPicker: null,
            resizeObserver: null,
            colorValue: '#e5e5e5',
        }
    },

    watch: {
        colorValue() {
            this.updateSelectedBox();
        },

        device: {
            deep: true,
            handler(newDevice: Device) {
                if (newDevice.equals(this.localDevice!)) {
                    return;
                }
                this.localDevice = new Device(newDevice);
            }
        }
    },

    created() {
        this.localDevice = new Device(this.device);
    },

    mounted() {
        this.colorPicker = this.initColorPicker();
        const box = this.localDevice!.updateNumberBox();
        if (box && this.device.status) {
            this.colorPicker!.color.set(box.color);
            this.colorValue = box.color;
        }
    },

    methods: {
        handleSelectBox(numberBox: number) {
            const box = this.localDevice!.updateNumberBox(numberBox);
            if (box && this.device.status) {
                this.colorPicker!.color.set(box.color);
                this.colorValue = box.color;
            }
            this.updateDevice();
        },

        updateSelectedBox() {
            const box = this.localDevice!.updateNumberBox();
            if (box && this.device.status && box.color !== this.colorValue) {
                box.color = this.colorValue;
                this.updateDevice();
            }
        },

        updateDevice() {
            this.$emit("updateDevice", this.localDevice);
        },

        selectDevice() {
            this.$emit("selectDevice");
        },

        initColorPicker(): IroColorPicker | null {
            const pickerElement = this.$refs.pickerRef as HTMLElement;
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

            this.resizeObserver = resizeObserver;
            return colorPickerInstance;
        },
    },

    beforeUnmount() {
        if (this.resizeObserver) {
            this.resizeObserver.disconnect();
        }
    },


});
</script>
