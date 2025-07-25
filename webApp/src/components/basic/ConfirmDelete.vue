<template>
    <div class="fixed inset-0 backdrop-blur-md  min-h-screen">
        <div class="w-10/12 p-4 mx-auto max-w-md h-full flex  items-center justify-center">
            <div class="bg-black/40 w-full p-5 gap-3 rounded-4xl flex flex-col justify-center">
                <span class="select-none font-bold  text-center text-lg"> Are you sure you want to delete {{ title }} ?</span>
                <button @click="() => { hasPermission(true) }"
                    class="bg-black mx-auto px-20 py-1 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 transition">
                    <span class="select-none text-center font-bold text-lg text-white/80">Yes</span>
                </button>
                <button @click="() => { hasPermission(false) }"
                    class="bg-red-600/90 mx-auto px-20 py-1 rounded-4xl hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)] hover:scale-105 transition ">
                    <span class="select-none text-center font-bold text-lg text-white">No</span> </button>
            </div>
        </div>
    </div>
</template>


<script lang="ts">
import { defineComponent } from 'vue';

export default defineComponent({
    name: 'ConfirmDelete',
    emits: ['hasPermission'],
    props: {
        title: {
            type: String,
            required: true,
        }

    },
    methods: {
        hasPermission(ok: boolean) {
            this.$emit('hasPermission', ok);
        },
    }
});
</script>
