<template>
    <div
        className="fixed px-5 py-10  inset-0 bg-black/30 backdrop-blur-md flex flex-col items-center gap-5 overflow-y-auto">
        <Header v-on:goBack="handleGoBack" v-on:addDevice="handleAddDevice"></Header>
        <DeviceItem @deviceSelected="deviceSelected" v-bind:devices="devices"></DeviceItem>

    </div>
</template>


<script lang="ts">
import type { Device } from '../../interface'; // Adjust the path as necessary
import Header from './Header.vue';
import DeviceItem from './DeviceItem.vue';

import { defineComponent } from 'vue';

export default defineComponent({
    name: 'DeviceMain',
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
        }
    }
});
</script>