<template>
    <label class="block mt-5 cursor-pointer h-6 w-full">
        <input @change="onSelect" type="radio" :checked="modelValue.valueOf() === value.valueOf()"  name="color" :value="value.valueOf()" class="hidden peer" />
        <div
            class="w-full h-full rounded-xl  transition duration-150  transform
             peer-checked:scale-110  bg-red-600/80 peer-checked:translate-y-[-3px] hover:scale-110 hover:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)] peer-checked:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)]">
            <span class="text-center font-bold text-white/90 block">OFF</span>
        </div>
    </label>
</template>


<script lang="ts">
export default {
    name: 'OffBox',
    emits: ['onSelect'],
    props: {
        value: {
            type: Number,
            required: true
        },
        modelValue: {
            type: Number,
            required: true,
        }
    },
    methods: {
        onSelect(event: Event) {
            const input = event.target as HTMLInputElement;
            this.$emit('onSelect', input.value);
        },
    }
};
</script>
