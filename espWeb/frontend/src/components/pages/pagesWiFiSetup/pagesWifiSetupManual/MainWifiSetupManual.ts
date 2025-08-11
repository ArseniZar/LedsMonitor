import { useState } from "preact/hooks";
import { StepWifiSetupManual } from "../../../../basic/types/Step";
import { html } from "../../../../html";
import { FormSsidAndPassword } from "./pageWifiSetupSsidAndPassword/FormSsidAndPassword";
import { Check } from "../basic/pageWifiSetupCheck/Check";
import { Network } from "../../../../basic/classes/Network";
import type { Callback } from "../../../../basic/types/Callback";
import { Start } from "../basic/pageWifiSetupStart/Start";

// prettier-ignore
export function MainWifiSetupManual(props: {connectEmit: Callback, goBack: Callback}) {
  // prettier-ignore
  const [stepStack, setStep] = useState<StepWifiSetupManual[]>([StepWifiSetupManual.Start]);
  const [network, setNetwork] = useState<Network | null>(null);

  function popStep() {
    setStep((prevSteps) => prevSteps.slice(0, -1));
  }

  function pushStep(newStep: StepWifiSetupManual) {
    setStep((prevSteps) => [...prevSteps, newStep]);
  }

  function handleGoBack() {
    if (stepStack.length - 1 > 0) {
      popStep();
    } else {
      props.goBack();
    }
  }

  function handleStart() {
    pushStep(StepWifiSetupManual.WifiAndPassword);
  }
  // prettier-ignore
  function handleSsidAndPassword({ ssid, password }: { ssid: string; password: string }) {
    const newNetwork = new Network({ssid:ssid, password:password});
    setNetwork(newNetwork);
    pushStep(StepWifiSetupManual.Check);
  }

  function handleConnect() {
    props.connectEmit(network);
    pushStep(StepWifiSetupManual.End);
  }
  // prettier-ignore
  return html` 
  ${[StepWifiSetupManual.Start].includes(stepStack[stepStack.length - 1])                 && html`<${Start} onStart=${handleStart} title=${"Manual Mode"}/>`}
  ${[StepWifiSetupManual.WifiAndPassword].includes(stepStack[stepStack.length - 1])       && html`<${FormSsidAndPassword} ssidAndPasswordEmit=${handleSsidAndPassword} />`}
  ${[StepWifiSetupManual.Check].includes(stepStack[stepStack.length - 1])                 && html`<${Check} network=${network} connectEmit=${handleConnect} goBack=${handleGoBack} />`}
  `;
}
