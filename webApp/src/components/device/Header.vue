<template>
    <div className="w-full flex flex-row justify-between items-center mb-6">
        <BackPage @click="goBack" />
        <div className="">
            <button 
                className="text-center text-2xl  font-bold text-white/90  hover:text-white/100 hover:scale-110  duration-200">
                Select device
            </button>
        </div>
        <DeviceAddButton @click="addDevice" />
    </div>
</template>

<script lang="ts">
import DeviceAddButton from './DeviceAddButton.vue';
import BackPage from './BackPage.vue';


import { defineComponent } from 'vue';
export default defineComponent({
    name: 'Header',

    components: {
        DeviceAddButton,
        BackPage
    },

    methods: {
        goBack() {
            this.$emit('goBack');
        },
        addDevice() {
            this.$emit('addDevice');
        }
    }
});

</script>