<template>
    <label class="block mt-5 cursor-pointer h-6 w-full">
        <input @change="onSelect" type="radio" :checked="modelValue === Number(colorBox.numberBox)" name="color" :value="colorBox.numberBox" class="hidden peer" />
        <div class="w-full h-full rounded-xl  transition duration-150  transform
             peer-checked:scale-110 peer-checked:translate-y-[-3px] hover:scale-110 hover:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)] peer-checked:shadow-[0_0_10px_1px_rgba(255,255,255,0.8)]"
            :style="{
                backgroundColor: colorBox.color,
            }">
            <span class="text-center font-bold text-white/90 block" :style="{
                color: getComplementaryHexColor(colorBox.color),
            }">{{ colorBox.color }}</span>
        </div>
    </label>
</template>

<script lang="ts">
import type { PropType } from 'vue';
import type { BoxColor } from '../../../../basic/classes/Device';
export default {
    name: 'ColorMainBox',
    emits: ['onSelect'],
    props: {
        colorBox: {
            type: Object as PropType<BoxColor>,
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
        }
        ,
        hexToRgba(hex: string, alpha: number): string {
            const c = hex.replace('#', '');
            const bigint = parseInt(c, 16);
            const r = (bigint >> 16) & 255;
            const g = (bigint >> 8) & 255;
            const b = bigint & 255;

            return `(${r},${g},${b},${alpha})`;
        },


        getComplementaryHexColor(hex: string): string {
            const cleanHex = hex.replace('#', '');
            const r = parseInt(cleanHex.slice(0, 2), 16);
            const g = parseInt(cleanHex.slice(2, 4), 16);
            const b = parseInt(cleanHex.slice(4, 6), 16);

            const rNew = 255 - r;
            const gNew = 255 - g;
            const bNew = 255 - b;

            const toHex = (value: number): string => {
                const hex = Math.round(value).toString(16);
                return hex.length === 1 ? '0' + hex : hex;
            };

            return `#${toHex(rNew)}${toHex(gNew)}${toHex(bNew)}`;
        }

    }
};
</script>
