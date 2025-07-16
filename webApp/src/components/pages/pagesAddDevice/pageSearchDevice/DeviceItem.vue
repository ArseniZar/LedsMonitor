<template>
    <div class="w-full flex flex-row gap-2.5 bg-black/30 p-2 rounded-xl ">
        <img src="../../../../assets/img/icons8-device-48.png" alt="Check" class="h-6" />
        <span class="w-full  font-bold"> {{item.name}} </span>
    </div>
</template>


<script lang="ts">
import { defineComponent } from 'vue';

export default defineComponent({
    name: 'DeviceItem',
    emits: [],
    props: {
        item: {
            type: Object as () => { id: string , name: string },
            required: true
        },
    },
    methods: {
        
    }
}
);
</script>