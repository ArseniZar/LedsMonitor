import { html } from "../../../../../html";
import type { ItemData } from "../../../../../basic/types/ItemData";
import type { Callback } from "../../../../../basic/types/Callback";

export function NetworkItem(props:{item: ItemData, onSelectEmit: Callback}) {
  function onSelect() {
    props.onSelectEmit(props.item.id);
  }

  return html`
    <button
      onClick=${onSelect}
      class="w-full flex flex-row bg-black/60 rounded-4xl p-2 px-6  hover:shadow-[0_0_10px_3px_rgba(255,255,255,0.8)]  active:bg-black hover:scale-105 transition"
    >
      <span class="w-full text-white text-left font-mono break-words block">
        ${props.item.title}
      </span>
      <img src=${props.item.imgIcon} alt="Check" class="h-6" />
    </button>
  `;
}
