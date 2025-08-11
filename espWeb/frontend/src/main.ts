import "./style.css";
import { MainWifiSetup } from "./components/pages/pagesWiFiSetup/MainWifiSetup";
import { html } from "./html";
import { render } from "preact";

function App() {
  return html`
    <div class="w-10/12  mx-auto max-w-md h-screen">
      <${MainWifiSetup} />
    </div>
  `;
}

render(html`<${App} />`, document.querySelector("#app")!);
