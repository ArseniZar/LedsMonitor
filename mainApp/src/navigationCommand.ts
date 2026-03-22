export const NavigationCommand = {
    OpenSettings: 'open-settings',
    OpenStartup: 'open-startup',
    OpenAddDevice: 'open-add-device',
    OpenColorPanel: 'open-color-panel',
    OpenSelectDevice: 'open-select-device',
    GoBack: 'go-back',
    NoChange: 'no-change',
} as const;

export type NavigationCommandKey = keyof typeof NavigationCommand;
export type NavigationCommandValue = typeof NavigationCommand[NavigationCommandKey];
