<template>
    <DeviceAddToken v-if="stepStack[stepStack.length - 1] === StepAddDevice.Token" @submitToken="handleTokenSubmit"
        @goBack="handleGoBack" />
    <DeviceAddName v-if="stepStack[stepStack.length - 1] === StepAddDevice.Name" @submitName="handleNameSubmit"
        @goBack="handleGoBack" />

</template>


<script lang="ts">
import { defineComponent } from 'vue';
import DeviceAddToken from './DeviceAddToken.vue';
import DeviceAddName from './DeviceAddName.vue';
import { StepAddDevice } from '../../../interface';
import type { Device, BoxColor } from '../../../interface';


export default defineComponent({
    name: 'DeviceAddMain',

    data(): {
        stepStack: StepAddDevice[],
        device: Device,
        StepAddDevice: typeof StepAddDevice,
    } {
        return {
            stepStack: [StepAddDevice.Token],
            device: {} as Device,
            StepAddDevice: StepAddDevice,
        };
    },
    components: {
        DeviceAddToken,
        DeviceAddName
    },
    methods: {
        handleTokenSubmit(token: string) {
            this.device.token = token;
            this.stepStack.push(StepAddDevice.Name);
        },
        handleNameSubmit(name: string) {
            this.device.name = name;
            this.stepStack.push(StepAddDevice.Devices);
            this.device.boxColorsList = this.createDefaultboxColors();
            this.device.boxColorMain = this.createDefaultboxColorsMain();
            this.$emit('deviceCreate', this.device);
        },
        handleGoBack() {
            this.stepStack.pop();
            if (this.stepStack.length === 0) {
                this.$emit('goBack');
            }
        },
        createDefaultboxColors(): BoxColor[] {
            return [
            { numberBox: 2, color: '#ffffff' },
            { numberBox: 3, color: '#ffa500' },
            { numberBox: 4, color: '#ff00ff' },
            { numberBox: 5, color: '#008000' },
            { numberBox: 6, color: '#8000ff' }];
        },
        createDefaultboxColorsMain(): BoxColor {
            return { numberBox: 1, color: '#e5e5e5' }
        }

    }
});
</script>
