<template>
    <template v-if="[StepSettings.Settings].includes(stepStack[stepStack.length - 1])">
        <HeaderBar :visibleMode="'back & add'" @goBack="handleGoBack" @goAdd="handleAddChat" />
        <div class="mt-7 w-full  h-full ">
            <CurrentSettingsChat :item="chatToItemData(currentChat, 'img/icons8-device-100.png')" />
        </div>
        <div class="mt-7 w-full  h-full flex  flex-col   bg-black/30 rounded-xl">
            <SettingsItem v-for="item in itemsChat" v-bind:visible="false"
                v-bind:item="chatToItemData(item, 'img/icons8-device-48.png')" @clickItem="handleSelectChat" />
        </div>
        <div class="mt-7 w-full h-full flex  bg-black/30 flex-col  rounded-xl">
            <SettingsItem v-bind:item="{ id: '1', imgIcon: 'img/icons8-device-48.png', title: 'Device' }"
                @clickItem="hadleDeviceList" />
        </div>
        <div class="mt-7 w-full h-full flex  bg-black/30 flex-col  rounded-xl">
            <SettingsItem v-bind:item="itemsConf[0]" @clickItem="" />
            <SettingsItem v-bind:item="itemsConf[1]" @clickItem="" />
        </div>
    </template>

    <DeviceList v-if="[StepSettings.Devices].includes(stepStack[stepStack.length - 1])" :devices="devices"
        @deleteDevice="handleDeleteDevice" @goBack="handleGoBack" />
</template>


<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import HeaderBar from '../../basic/header/HeaderBar.vue';
import SettingsItem from './pageSettings/SettingsItem.vue';
import CurrentSettingsChat from './pageSettings/CurrentSettingsChat.vue';
import { StepSettings, type Device, type ItemData } from '../../../interface'
import DeviceList from './pageDevice/DeviceList.vue';
import type { Chat } from '../../../telegram/types';
export default defineComponent({
    name: 'MainSettings',
    components: {
        SettingsItem,
        CurrentSettingsChat,
        HeaderBar,
        DeviceList,

    },
    emits: ['goBack', 'selectChat', 'deleteDevice' , 'addChat'],
    props: {
        itemsChat: {
            type: Array as PropType<Chat[]>,
            required: true,
        },
        devices: {
            type: Array as PropType<Device[]>,
            required: true,
        },
        currentChat: {
            type: Object as PropType<Chat>,
            required: true,
        }

    },
    data(): {
        stepStack: StepSettings[];
        StepSettings: typeof StepSettings;
        itemsConf: Array<ItemData>;
    } {
        return {
            stepStack: [StepSettings.Settings],
            StepSettings: StepSettings,


            itemsConf: [
                { id: '0', imgIcon: 'img/icons8-key-64.png', title: 'Token' },
                { id: '0', imgIcon: 'img/icons8-data-64.png', title: 'Data' },
            ]

        };
    },


    methods: {

        chatToItemData(chat: Chat, imgIcon: string): ItemData {
            return {
                id: chat.id,
                imgIcon: chat.imgIcon !== undefined ? chat.imgIcon : imgIcon,
                title: chat.title,
            };
        },

        findChatById(itemsChat: Chat[], id: string): Chat | undefined {
            return itemsChat.find(chat => String(chat.id) === String(id));
        },


        hadleDeviceList() {
            this.stepStack.push(StepSettings.Devices);
        },

        handleDeleteDevice(device: Device) {
            this.$emit('deleteDevice', device);
        },

        handleGoBack() {
            if (this.stepStack.length - 1 == 0) {
                this.$emit('goBack');
                return;
            }
            this.stepStack.pop();
        },

        handleSelectChat(id: string) {
            const chat = this.findChatById(this.itemsChat, id);
            if (chat !== undefined && (!this.currentChat || chat.id !== this.currentChat.id)) {
                this.$emit('selectChat', chat);
            }
        },

        handleAddChat() {
            this.$emit('addChat');
        }


    },
});
</script>