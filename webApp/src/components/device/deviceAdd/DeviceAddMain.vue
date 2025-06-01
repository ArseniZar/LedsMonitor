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
import type { Device } from '../../../interface';


export default defineComponent({
    name: 'DeviceAddMain',

    data() {
        return {
            stepStack: [StepAddDevice.Token] as StepAddDevice[],
            device: {} as Device,
            StepAddDevice: StepAddDevice,
        };
    },
    components: {
        DeviceAddToken,
        DeviceAddName
    },

    watch: {
        // stepStack(newStack) {
        //     const last = newStack[newStack.length - 1];
        //     if (last === StepAddDevice.Devices) {
        //         console.log("All steps completed, device created");
        //         this.$emit('deviceCreate');
        //         // this.onDevicesStepReached();
        //     }
        // }
    },
    methods: {
        handleTokenSubmit(token: string) {
            this.device.token = token;
            this.stepStack.push(StepAddDevice.Name);
        },
        handleNameSubmit(name: string) {
            this.device.name = name;
            this.stepStack.push(StepAddDevice.Devices);
            // console.log("All steps completed, device created");
            this.$emit('deviceCreate', this.device);
        },
        handleGoBack() {
            if (this.stepStack.length === 0) {
                console.warn("No steps to go back to.");
                return;
            }
            this.stepStack.pop();
        }

    }
});
</script>
