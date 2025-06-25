<template>
    <div className="fixed inset-0 backdrop-blur-md  bg-black/40 overflow-y-auto w-full  min-h-screen">
        <div className="py-10  mx-auto w-8/12  max-w-md  h-full  flex flex-col items-center gap-3.5 ">
            <Header v-on:goBack="handleGoBack" v-on:addDevice="handleAddDevice"></Header>
            <DeviceItem @deviceSelected="deviceSelected" v-bind:device="device" v-on:deviceDelete="handleDeleteDevice"
                v-for="device in devices"></DeviceItem>
        </div>
    </div>
</template>


<script lang="ts">
import type { Device } from '../../interface'; // Adjust the path as necessary
import Header from './Header.vue';
import DeviceItem from './DeviceItem.vue';

import { defineComponent } from 'vue';

export default defineComponent({
    name: 'DeviceMain',
    emits: ['goBack', 'addDevice', 'deviceSelected', 'deleteDevice'],
    components: {
        Header,
        DeviceItem
    },
    props: {
        devices: {
            type: Array as () => Device[],
            required: true
        }
    },
    methods: {
        deviceSelected(device: Device) {
            this.$emit('deviceSelected', device);
        },
        handleGoBack() {
            this.$emit('goBack');
        },
        handleAddDevice() {
            this.$emit('addDevice');
        },
        handleDeleteDevice(device: Device) {
            this.$emit('deleteDevice', device)
        }
    }
});
</script>