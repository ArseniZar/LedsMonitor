<template>
    <div className="fixed  w-full left-1/2 bottom-8 z-10 -translate-x-1/2">
        <div class="w-10/12 max-w-md mx-auto">
            <div class=" flex flex-row  p-2 rounded-4xl  bg-black/20 justify-between gap-5 ">
                <BottomButton :checked="false" class="ml-5 " @event="goBack" v-bind:data="bottomButtons[0]" />
                <BottomButton :checked="visible === StepMain.Main" @event="goMain" v-bind:data="bottomButtons[1]" />
                <BottomButton :checked="visible === StepMain.Settings" class="mr-5" @event="goSettings"
                    v-bind:data="bottomButtons[2]" />
            </div>
        </div>
    </div>
</template>



<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import BottomButton from './BottomButton.vue';
import type { ItemData } from '../../../basic/types/itemData';
import { StepMain } from '../../../basic/types/steps';

export default defineComponent({
    name: 'BottomBar',
    emits: ['goBack', 'goMain', 'goSettings'],
    components: {
        BottomButton
    },
    props: {
        visible: {
            type: String as PropType<StepMain>,
            required: true
        }
    },
    data(): {
        StepMain: typeof StepMain;
        bottomButtons: Array<ItemData>;
    } {
        return {
            StepMain: StepMain,
            bottomButtons: [
                { id: '1', imgIcon: 'img/icons8-ped-50.png', title: '' },
                { id: '2', imgIcon: 'img/icons8-home-48.png', title: '' },
                { id: '3', imgIcon: 'img/icons8-settings-48.png', title: '' },]
        };
    }
    ,
    methods: {
        goBack() {
            this.$emit('goBack');
        },
        goMain() {
            this.$emit('goMain');
        },
        goSettings() {
            this.$emit('goSettings');
        }
    }
});
</script>
