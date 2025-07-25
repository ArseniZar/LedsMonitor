import type { ItemData } from "../types/itemData";

export class Chat {
    id: string;
    title: string;
    imgIcon?: string;
    type?: string;

    constructor(chat: Chat);
    constructor(id: string, title: string, imgIcon?: string, type?: string);
    constructor(
        idOrChat: string | Chat,
        title?: string,
        imgIcon?: string,
        type?: string
    ) {
        if (typeof idOrChat === 'string') {
            this.id = idOrChat;
            this.title = title!;
            this.imgIcon = imgIcon;
            this.type = type;
        } else {
            this.id = idOrChat.id;
            this.title = idOrChat.title;
            this.imgIcon = idOrChat.imgIcon;
            this.type = idOrChat.type;
        }
    }

    toItemData(defaultImgIcon: string): ItemData {
        return {
            id: this.id,
            imgIcon: this.imgIcon !== undefined ? this.imgIcon : defaultImgIcon,
            title: this.title,
        };
    }


    isGroup(): boolean {
        return this.type === 'group';
    }

    equals(other: Chat): boolean {
        return this.id === other.id;
    }
}