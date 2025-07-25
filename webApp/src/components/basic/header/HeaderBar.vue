<template>
    <div class="w-full flex flex-row justify-between">

        <HeaderButton v-if="visibleMode === 'back & change' || visibleMode === 'back & add' || visibleMode === 'back'" @event="goBack"
            v-bind:data="headerButtons[0]" />
        <HeaderButton v-if="visibleMode === 'back & change' || visibleMode === 'change'" @event="goChange"
            v-bind:data="headerButtons[1]" />
        <HeaderButton v-if="visibleMode === 'back & add' || visibleMode === 'add'" @event="goAdd"
            v-bind:data="headerButtons[2]" />
    </div>
</template>

<script lang="ts">
import { defineComponent, type PropType } from 'vue';
import HeaderButton from './HeaderButton.vue';
import type { ItemData } from '../../../basic/types/itemData';

export default defineComponent({
    name: 'HeaderBar',
    emits: ['goBack', 'goChange','goAdd'],
    components: {
        HeaderButton
    },
    props: {
        visibleMode: {
            type: String as PropType<'back' | 'change' | 'add' | 'back & change' | 'back & add'>,
            required: true
        }
    },
    data(): {
        headerButtons: Array<ItemData>;
    } {
        return {
            headerButtons: [
                { id: '1', imgIcon: 'img/icons8-ped-50.png', title: '' },
                { id: '2', imgIcon: '', title: 'chg.' },
                { id: '3', imgIcon: 'img/icons8-add-24.png', title: '' },]
        };
    }
    ,
    methods: {
        goBack() {
            this.$emit('goBack');
        },
        goChange() {
            this.$emit('goChange');
        },
        goAdd(){
            this.$emit('goAdd');
        }
    }
});
</script>