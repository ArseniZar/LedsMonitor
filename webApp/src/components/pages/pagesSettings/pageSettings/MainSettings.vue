<template>
    <HeaderBar :visibleMode="'back & add'" @goBack="handleGoBack" />
    <div  v-if="itemsCurrentChat" class="mt-7 w-full  h-full flex items-center gap-1 flex-col  rounded-xl">
        <img src="/src/assets/img/icons8-device-100.png" alt="Check" class="h-20" />
        <span class="font-bold text-center text-2xl"> {{ itemsCurrentChat.title }}</span>
    </div>
    <div class="mt-7 w-full  h-full flex  flex-col   bg-black/30 rounded-xl">
        <SettingsItem v-for="item in itemsChat" v-bind:item="item" @clickItem="" />
    </div>
    <div  v-if="itemsCurrentChat" class="mt-7 w-full h-full flex  bg-black/30 flex-col  rounded-xl">
        <SettingsItem v-bind:item="itemDevice" @clickItem="" />
    </div>
    <div class="mt-7 w-full h-full flex  bg-black/30 flex-col  rounded-xl">
        <SettingsItem v-bind:item="itemsConf[0]" @clickItem="" />
        <SettingsItem v-bind:item="itemsConf[1]" @clickItem="" />
    </div>

</template>


<script lang="ts">
import { defineComponent } from 'vue';
import HeaderBar from '../../../basic/header/HeaderBar.vue';
import SettingsItem from './SettingsItem.vue';
import type { ItemData } from '../../../../interface'
export default defineComponent({
    name: 'Settings',
    components: {
        SettingsItem,
        HeaderBar,

    },
    emits: ['goBack'],
    props: {},
    data(): {

        itemsChat: Array<ItemData>;
        itemsCurrentChat: ItemData | null;
        itemDevice: ItemData;
        itemsConf: Array<ItemData>;
    } {
        return {
            itemsChat: [
                { id: 1, imgIcon: 'src/assets/img/icons8-chat-24.png', title: 'chat1' },
                // { id: 2, imgIcon: 'src/assets/img/icons8-chat-24.png', title: 'chat2' },
                // { id: 3, imgIcon: 'src/assets/img/icons8-chat-24.png', title: 'chat3' },
            ],
            itemsCurrentChat: null,
            itemDevice: {
                id: 1, imgIcon: 'src/assets/img/icons8-device-48.png', title: 'Device'
            },
            itemsConf: [
                { id: 0, imgIcon: 'src/assets/img/icons8-key-64.png', title: 'Token' },
                { id: 0, imgIcon: 'src/assets/img/icons8-data-64.png', title: 'Data' },
            ]

        };
    },
    methods: {
        handleGoBack() {
            this.$emit('goBack');
        }
    },
});
</script>