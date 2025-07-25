<template>
    <div className="fixed inset-0 backdrop-blur-md  bg-black/40 overflow-y-auto w-full  min-h-screen">
        <div className="py-10  mx-auto w-8/12  max-w-md  h-full  flex flex-col items-center gap-3.5 ">
            <HeaderBar :visibleMode="'back'" @goBack="handleGoBack" />
            <DeviceItem @deviceSelected="deviceSelected" v-for="device in devices" v-bind:item="device.toItemData()">
            </DeviceItem>
        </div>
    </div>
</template>


<script lang="ts">
import type { Device } from '../../../../basic/classes/Device';
import DeviceItem from './SelectDeviceItem.vue';
import HeaderBar from '../../../basic/header/HeaderBar.vue';

import { defineComponent } from 'vue';

export default defineComponent({
    name: 'MainSelectDevice',
    emits: ['goBack', 'deviceSelected'],
    components: {
        DeviceItem,
        HeaderBar,
    },
    props: {
        devices: {
            type: Array as () => Device[],
            required: true
        }
    },
    methods: {
        findDeviceById(devices: Device[], id: string): Device | undefined {
            return devices.find(device => String(device.id) === String(id));
        },

        deviceSelected(id: string) {
            const device = this.findDeviceById(this.devices, id);
            this.$emit('deviceSelected', device);
        },

        handleGoBack() {
            this.$emit('goBack');
        },
    }
});
</script>