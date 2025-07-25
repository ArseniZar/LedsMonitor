<template>
    <template v-if="[StepSettings.Settings].includes(stepStack[stepStack.length - 1])">
        <HeaderBar :visibleMode="'back & add'" @goBack="handleGoBack" @goAdd="handleAddChat" />
        <div class="mt-7 w-full  h-full ">
            <CurrentSettingsChat
                :item="currentChat.toItemData('img/icons8-device-100.png')" />
        </div>
        <div class="mt-7 w-full  h-full flex  flex-col   bg-black/30 rounded-xl">
            <SettingsItem v-for="item in itemsChat" v-bind:visible="false"
                v-bind:item="item.toItemData('img/icons8-chat-24.png')" @clickItem="handleSelectChat"
                @longPressItem="handleDeleteChat" />
        </div>
        <div class="mt-7 w-full h-full flex  bg-black/30 flex-col  rounded-xl">
            <SettingsItem v-bind:item="{ id: '1', imgIcon: 'img/icons8-device-48.png', title: 'Device' }"
                @clickItem="hadleSelectDeviceList" />
        </div>
        <div class="mt-7 w-full h-full flex  bg-black/30 flex-col  rounded-xl">
            <SettingsItem v-bind:item="{ id: '0', imgIcon: 'img/icons8-key-64.png', title: 'Token' }"
                @clickItem="hadleSelectToken" />
            <SettingsItem v-bind:item="{ id: '0', imgIcon: 'img/icons8-data-64.png', title: 'Data' }" @clickItem="" />
        </div>
    </template>
    <ConfirmDelete v-if="chatToDelete" :title="chatToDelete.title" @hasPermission="hasPermission" />
    <DeviceList v-if="[StepSettings.Devices].includes(stepStack[stepStack.length - 1])" :devices="devices!"
        @deleteDevice="handleDeleteDevice" @goBack="handleGoBack" />
    <Token v-if="[StepSettings.Token].includes(stepStack[stepStack.length - 1])" @deleteToken="handleDeleteToken"
        @goBack="handleGoBack" />

</template>


<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import HeaderBar from '../../basic/header/HeaderBar.vue';
import SettingsItem from './pageSettings/SettingsItem.vue';
import CurrentSettingsChat from './pageSettings/CurrentSettingsChat.vue';
import DeviceList from './pageDevice/DeviceList.vue';
import Token from './pageToken/Token.vue';
import ConfirmDelete from '../../basic/ConfirmDelete.vue';

import { StepSettings } from '../../../basic/types/steps'
import { Device } from '../../../basic/classes/Device'
import { Chat } from '../../../basic/classes/Chat';


export default defineComponent({
    name: 'MainSettings',
    components: {
        SettingsItem,
        CurrentSettingsChat,
        HeaderBar,
        DeviceList,
        Token,
        ConfirmDelete


    },
    emits: ['goBack', 'chatToSelect', 'deviceToRemove', 'tokenToRemove', 'chatToRemove', 'chatToAdd'],
    props: {
        itemsChat: {
            type: Array as PropType<Chat[] >,
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
        chatToDelete: Chat | null;
    } {
        return {
            stepStack: [StepSettings.Settings],
            StepSettings: StepSettings,
            chatToDelete: null,

        };
    },

    methods: {

        findChatById(itemsChat: Chat[], id: string): Chat | undefined {
            return itemsChat.find(chat => chat.id === id);
        },

        handleSelectChat(id: string) {
            const chat = this.findChatById(this.itemsChat!, id);
            if (chat && (!this.currentChat || !chat.equals(this.currentChat))) {
                this.$emit('chatToSelect', chat);
            }
        },

        hadleSelectToken() {
            this.stepStack.push(StepSettings.Token);
        },

        hadleSelectDeviceList() {
            this.stepStack.push(StepSettings.Devices);
        },

        handleDeleteDevice(device: Device) {
            this.$emit('deviceToRemove', device);
        },

        handleDeleteToken() {
            this.$emit('tokenToRemove');
        },

        handleDeleteChat(id: string) {
            const chat = this.findChatById(this.itemsChat!, id);
            if (chat) {
                this.chatToDelete = chat;
            }
        },

        handleAddChat() {
            this.$emit('chatToAdd');
        },

        hasPermission(isAllowed: boolean) {
            if (isAllowed && this.chatToDelete) {
                this.$emit('chatToRemove', this.chatToDelete);
            }
            this.chatToDelete = null;
        },

        handleGoBack() {
            if (this.stepStack.length - 1 === 0) {
                this.$emit('goBack');
            } else {
                this.stepStack.pop();
            }
        },


    },
});
</script>