<template>
    <HeaderBar :visible-mode="'back'" @goBack="goBack" />
    <div class="mt-5 w-full h-full flex flex-col gap-3">
        <DeviceItem v-for="item in devices" :device="deviceToItemData(item, 'img/icons8-device-48.png')"
            @deleteDevice="deleteDevice" />
    </div>
    <ConfirmDelete v-if="deviceToDelete" :title="deviceToDelete.name" @hasPermission="hasPermission" />

</template>

<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import type { ItemData, Device } from '../../../../interface';
import DeviceItem from './DeviceItem.vue';
import HeaderBar from '../../../basic/header/HeaderBar.vue';
import ConfirmDelete from './ConfirmDelete.vue';
export default defineComponent({
    name: 'Device',
    components: {
        DeviceItem,
        HeaderBar,
        ConfirmDelete,
    },
    emits: ['deleteDevice', 'goBack'],
    props: {
        devices: {
            type: Array as PropType<Device[]>,
            required: true,
        }

    },
    data(): {
        deviceToDelete: Device | null;
    } {
        return {
            deviceToDelete: null,
        }
    },
    methods: {

        deviceToItemData(device: Device, imgIcon: string): ItemData {
            return {
                id: device.id,
                imgIcon: device.imgIcon !== undefined ? device.imgIcon : imgIcon,
                title: device.name,
            };
        },

        findDeviceById(devices: Device[], id: string): Device | undefined {
            return devices.find(device => String(device.id) === String(id));
        },


        deleteDevice(id: string) {
            const device = this.findDeviceById(this.devices, id);
            if (device !== undefined) {
                this.deviceToDelete = device;
            }
        },


        hasPermission(isAllowed: boolean) {
            if (isAllowed && this.deviceToDelete) {
                this.$emit('deleteDevice', this.deviceToDelete);
            }
            this.deviceToDelete = null;
        },

        goBack() {
            this.$emit('goBack');
        },
    }
});
</script>