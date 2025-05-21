import { IChat, IMessage } from "@/types"
import { create } from "zustand"

interface ChatsListStore {
    data: IChat[]
    lastMessage: IMessage | null
    setChatsList: (chat: IChat[]) => void
    setLastMessage: (chat: IChat, message: IMessage) => void
    getLastMessage: (chat: IChat) => string | null
}

export const useChatsListStore = create<ChatsListStore>()((set, get) => ({
    data: [],
    lastMessage: null,
    setChatsList: (chats: IChat[]) => set({ data: chats }),
    setLastMessage: (chat: IChat, message: IMessage) => {
        const { data } = get()
        const current_chat = data.find((c) => c.id === chat.id)
        if (!current_chat) return
        current_chat.lastMessage = message
        if (!current_chat) return
        const new_chats = data.filter((c) => c.id !== chat.id)
        set({ data: [...new_chats, current_chat] })
    },
    getLastMessage: (chat: IChat) => {
        const { data } = get()
        const lastMessage = data.find((c) => c.id === chat.id)?.lastMessage?.body
        return lastMessage ? lastMessage : null
    },
}))
