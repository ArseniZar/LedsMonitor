export const ComponentId = {
    BottomBarMain: 'handleBottomBarMain',
    BottomBarSettings: 'handleBottomBarSettings',
    MainStartup: 'handleMainStartup',
    MainAddDevice: 'MainAddDevice',
    MainColorPanel: 'handleMainColorPanel',
    MainSelectDevice: 'MainSelectDevice',
    MainSettings: 'MainSettings',
    GoBack: 'handleGoBack',
} as const;

export type ComponentIdType = keyof typeof ComponentId;
export type ComponentIdValueType = typeof ComponentId[ComponentIdType];
