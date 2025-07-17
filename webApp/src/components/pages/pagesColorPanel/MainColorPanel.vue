<template>
    <SelectDeviceButton :title="localDevice.name" @selectDevice="selectDevice" />

    <div className="flex justify-center items-center mt-5 md:mt-10 g:mt-20 w-full">
        <div ref="pickerRef" class="w-full"></div>
    </div>
    <ColorMainBox :colorBox="localDevice.boxColorMain" :modelValue="localDevice.currentNumberBox.valueOf()"
        v-on:onSelect="handleSelectBox" />

    <div class="flex  w-full flex-row justify-between gap-3 mt-5  mb-5">
        <ColorBox v-for="box in localDevice.boxColorsList" :colorBox="box"
            :modelValue="localDevice.currentNumberBox.valueOf()" v-on:onSelect="handleSelectBox" />
    </div>

    <div class="w-full flex items-center mt-5 mb-5">
        <OffBox :colorBox="localDevice.boxColorOff" :modelValue="localDevice.currentNumberBox.valueOf()"
            v-on:onSelect="handleSelectBox" />
    </div>
</template>


<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import iro from '@jaames/iro';
import { Device } from '../../../interface';
import ColorMainBox from './pageColorPanel/panelElements/ColorMainBox.vue';
import ColorBox from './pageColorPanel/panelElements/ColorBox.vue';
import OffBox from './pageColorPanel/panelElements/OffBox.vue';
import SelectDeviceButton from './pageColorPanel/panelElements/SelectDeviceButton.vue';
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
            handler(newDevice: Device) {
                this.localDevice = new Device(newDevice);
            }
        },

        colorValue() {
            this.updateSelectedBoxColor();
        },
    },

    mounted() {
        this.initColorPicker();
        this.updateDevice();


    },

    methods: {
        handleSelectBox(numberBox: number) {
            this.localDevice.currentNumberBox = Number(numberBox);
            this.updateDevice();
        },

        updateSelectedBoxColor() {
            if (!this.localDevice.currentNumberBox) return;

            const currentBoxNumber = Number(this.localDevice.currentNumberBox);

            let box = this.localDevice.boxColorsList.find(
                b => Number(b.numberBox) === currentBoxNumber
            );

            if (!box && Number(this.localDevice.boxColorMain.numberBox) === currentBoxNumber) {
                box = this.localDevice.boxColorMain;
            }

            if (box && box.color !== this.colorValue) {
                box.color = this.colorValue;
                this.updateDevice();
            }
        },

        updateDevice() {
            const newStatus = Number(this.localDevice.currentNumberBox) === Number(0);
            if (this.localDevice.status !== newStatus) {
                this.localDevice.status = newStatus;
            }
            this.$emit("updateDevice", this.localDevice);
        },

        selectDevice() {
            this.$emit("selectDevice");
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
