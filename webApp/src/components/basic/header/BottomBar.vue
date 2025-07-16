<template>
    <div className="fixed  w-full left-1/2 bottom-8  -translate-x-1/2">
        <div class="w-10/12 max-w-md mx-auto">
            <div class=" flex flex-row  p-2 rounded-4xl  bg-black/20 justify-between gap-5 ">
                <BottomButton :checked="visible === 'back'" class="ml-5 " @event="goBack"
                    v-bind:data="bottomButtons[0]" />

                <BottomButton :checked="visible === 'main'" @event="goMain" v-bind:data="bottomButtons[1]" />

                <BottomButton :checked="visible === 'settings'" class="mr-5" @event="goSettings"
                    v-bind:data="bottomButtons[2]" />
            </div>
        </div>
    </div>
</template>



<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import BottomButton from './BottomButton.vue';
import type { ItemData} from '../../../interface';

export default defineComponent({
    name: 'BottomBar',
    emits: ['goBack', 'goMain', 'goSettings'],
    components: {
        BottomButton
    },
    props: {
        visible: {
            type: String as PropType<'back' | 'main' | 'settings'>,
            required: true
        }
    },
    data(): {
        bottomButtons: Array<ItemData>;
    } {
        return {
            bottomButtons: [
                { id: 1, imgIcon: 'img/icons8-ped-50.png', title: '' },
                { id: 2, imgIcon: 'img/icons8-home-48.png', title: '' },
                { id: 3, imgIcon: 'img/icons8-settings-48.png', title: '' },]
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
