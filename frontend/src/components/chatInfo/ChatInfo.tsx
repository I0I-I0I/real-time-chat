import { Button, Typography } from "@/components/UI"
import { useChatStore } from "@/state/chat"
import { useUserStore } from "@/state/user"
import cls from "@/utils/cls"
import { forwardRef } from "react"
import { ReactSVG } from "react-svg"
import styles from "./ChatInfo.module.css"

interface ChatInfoProps {
    className?: string
    onClickCloseMobile?: () => void
}

export const ChatInfo = forwardRef<HTMLDivElement, ChatInfoProps>(
    ({ className = "", onClickCloseMobile }, ref) => {
        const currentChat = useChatStore((state) => state.data)
        const currentUserLogin = useUserStore((state) => state.data?.login)

        return (
            <div className={cls(styles.chat_info, className)} ref={ref}>
                <Button
                    className={styles.return_button}
                    variant="icon"
                    onClick={onClickCloseMobile}
                >
                    <ReactSVG src="/back.svg" />
                </Button>
                {currentChat && (
                    <>
                        <ReactSVG className={styles.icon} src="/account.svg" />
                        <Typography className={styles.name} tag="h2" variant="title-4">
                            {currentChat.type === "friend"
                                ? currentUserLogin !== currentChat.name2
                                    ? currentChat.name2
                                    : currentChat.name
                                : currentChat.name}
                        </Typography>
                        <Typography className={styles.last_seen} tag="span" variant="text_small">
                            last seen
                        </Typography>
                    </>
                )}
            </div>
        )
    }
)
