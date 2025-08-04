import { restart } from "../../basic/img/img";
import type { Callback } from "../../basic/types/Callback";
import { html } from "../../html";

export function Restart(props:{onRestartEmit: Callback}) {
  return html`
    <button>
      <img onClick=${props.onRestartEmit} src=${restart} alt="Check" class="mt-4 h-8" />
    </button>
  `;
}
