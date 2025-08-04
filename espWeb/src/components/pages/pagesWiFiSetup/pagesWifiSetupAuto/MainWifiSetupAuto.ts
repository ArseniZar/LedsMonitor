import { useState } from "preact/hooks";
import { html } from "../../../../html";
import { Network } from "../../../../basic/classes/Network";
import { Start } from "../basic/pageWifiSetupStart/Start";
import { StepWifiSetupAuto } from "../../../../basic/types/Step";
import { SearchNetwork } from "./pageWifiSetupNetwork/SearchNetwork";
import { FormPassword } from "./pageWifiSetupPassword/FormPassword";
import type { Callback } from "../../../../basic/types/Callback";
import { Check } from "../basic/pageWifiSetupCheck/Check";

export function MainWifiSetupAuto(props: {
  connectEmit: Callback;
  goBack: Callback;
}) {
  // prettier-ignore
  const [stepStack, setStep] = useState<StepWifiSetupAuto[]>([StepWifiSetupAuto.Start]);
  const [network, setNetwork] = useState<Network | null>(null);

  function popStep() {
    setStep((prevSteps) => prevSteps.slice(0, -1));
  }

  function pushStep(newStep: StepWifiSetupAuto) {
    setStep((prevSteps) => [...prevSteps, newStep]);
  }

  function handleStart() {
    pushStep(StepWifiSetupAuto.SearchNetwork);
  }

  function handleSelectNetwork(network: Network) {
    setNetwork(network);
    pushStep(StepWifiSetupAuto.Password);
  }

  function handleAddPassword(password: string) {
    if (network !== null) {
      const newNetwork = new Network(network);
      newNetwork.password = password;
      setNetwork(newNetwork);
      pushStep(StepWifiSetupAuto.Check);
    }
  }

  function handleConnect() {
    props.connectEmit(network);
    pushStep(StepWifiSetupAuto.End);
  }

  function handleGoBack() {
    if (stepStack.length - 1 > 0) {
      popStep();
    } else {
      props.goBack();
    }
  }

  // prettier-ignore
  return html`
    ${[StepWifiSetupAuto.Start].includes(stepStack[stepStack.length - 1])         && html`<${Start} onStart=${handleStart} title=${"Auto Mode"}/>`}
    ${[StepWifiSetupAuto.SearchNetwork].includes(stepStack[stepStack.length - 1]) && html`<${SearchNetwork} selectNetworkEmit=${handleSelectNetwork} />`}
    ${[StepWifiSetupAuto.Password].includes(stepStack[stepStack.length - 1])      && html`<${FormPassword} addPasswordEmit=${handleAddPassword} goBack=${handleGoBack} />`}
    ${[StepWifiSetupAuto.Check].includes(stepStack[stepStack.length - 1])         && html`<${Check} network=${network} connectEmit=${handleConnect} goBack=${handleGoBack} />`}
  `;
}
